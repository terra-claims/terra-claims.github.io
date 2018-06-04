[%bs.raw {|require('./App.css')|}];
open Properties;
open Belt;

type state = {
  properties: properties,
  current: option(property),
  map: option(Leaflet.Map.t),
};

type action =
  | SetMap(Leaflet.Map.t)
  | AddProperty(property)
  | RemoveProperty(property)
  | SelectProperty(property)
  | LocateMe
  | UnselectProperty;

let hashes = [
  "QmV1zkqcFvnNWcAkCcDn6xxsj1rwszuS8mzhsDrTWxZ6Gk",
  "QmU4GnR8vZVVwYEGafffTm8sCJw3tk96Ap5YBym1bXgKKg",
];

let component = ReasonReact.reducerComponent("App");

let s = ReasonReact.string;

let make = _children => {
  ...component,
  initialState: () => {properties: [], current: None, map: None},
  reducer: (action, state) =>
    switch (action) {
    | SetMap(map) => ReasonReact.Update({...state, map: Some(map)})
    | SelectProperty(prop) =>
      ReasonReact.Update({...state, current: Some(prop)})
    | UnselectProperty => ReasonReact.Update({...state, current: None})
    | LocateMe =>
      ReasonReact.SideEffects(
        (
          self =>
            switch (self.state.map) {
            | None => ()
            | Some(map) =>
              map
              |. Leaflet.Map.locate(
                   Leaflet.locate_options(~setView=true, ()),
                 )
              |. ignore
            }
        ),
      )
    | AddProperty(prop) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, properties: [prop, ...state.properties]},
        (
          self =>
            switch (self.state.map) {
            | None => ()
            | Some(map) =>
              map
              |. Leaflet.Map.addGeoJSON(prop.layer)
              |. Leaflet.Map.fitBounds(
                   Leaflet.GeoJSON.getBounds(prop.layer),
                 )
              |. ignore
            }
        ),
      )
    | RemoveProperty(prop) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, properties: List.keep(state.properties, p => p != prop)},
        (
          self =>
            switch (self.state.map) {
            | None => ()
            | Some(map) =>
              map |. Leaflet.Map.removeGeoJSON(prop.layer) |. ignore
            }
        ),
      )
    },
  render: self =>
    <div className="App">
      <MaterialUI.CssBaseline />
      <MaterialUI.AppBar position=`Static color=`Inherit>
        <MaterialUI.Toolbar>
          <MaterialUI.IconButton onClick=(_ => self.send(LocateMe))>
            <MaterialUI.Icon color=`Primary>
              (s("location_searching"))
            </MaterialUI.Icon>
          </MaterialUI.IconButton>
          <MaterialUI.Typography
            variant=`Title color=`Inherit className="Title">
            (s("Terra"))
          </MaterialUI.Typography>
          <IpfsField addProperty=(property => self.send(AddProperty(property)))/>
        </MaterialUI.Toolbar>
      </MaterialUI.AppBar>
      <MapView setMap=(map => self.send(SetMap(map))) />
    </div>,
};