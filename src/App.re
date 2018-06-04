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
  | RemoveProperty(string)
  | SelectProperty(string)
  | LocateMe
  | UnselectProperty;

let component = ReasonReact.reducerComponent("App");

let s = ReasonReact.string;

let make = _children => {
  ...component,
  initialState: () => {properties: Map.String.empty, current: None, map: None},
  reducer: (action, state) =>
    switch (action) {
    | SetMap(map) => ReasonReact.Update({...state, map: Some(map)})
    | SelectProperty(hash) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, current: Map.String.get(state.properties, hash)},
        self => switch(Map.String.get(state.properties, hash)){
        | None => ()
        | Some(prop) => switch(state.map) {
          | None => ()
          | Some(map) => map |. Leaflet.Map.fitBounds(Leaflet.GeoJSON.getBounds(prop.layer)) |. ignore
          }
        })
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
        {...state, properties: Map.String.set(state.properties, prop.ipfsHash, prop), current: Some(prop)},
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
    | RemoveProperty(ipfsHash) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, properties: Map.String.remove(state.properties, ipfsHash)},
        (
          self =>
            switch (self.state.map) {
            | None => ()
            | Some(map) =>
              switch (Map.String.get(state.properties, ipfsHash)) {
              | None => ()
              | Some(prop) => map |. Leaflet.Map.removeGeoJSON(prop.layer) |. ignore
              }
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
          <IpfsField addProperty=(property => self.send(AddProperty(property))) selectProperty=(property => self.send(SelectProperty(property)))  properties=self.state.properties/>
        </MaterialUI.Toolbar>
      </MaterialUI.AppBar>
      <MapView setMap=(map => self.send(SetMap(map))) />
    </div>,
};
