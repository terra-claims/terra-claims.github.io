[%bs.raw {|require('./App.css')|}];
open Belt;

type property = {
  hash: string,
  geoJSON: Js.Json.t,
  layer: Leaflet.GeoJSON.t
};

type state = {
  properties: list(property),
  current: option(property),
  map: option(Leaflet.Map.t)
};

type action =
  | SetMap(Leaflet.Map.t)
  | AddProperty(property)
  | RemoveProperty(property)
  | SelectProperty(property)
  | UnselectProperty;

let component = ReasonReact.reducerComponent("App");

let s = ReasonReact.string;

let make = (_children) => {
  ...component,
  initialState: () => {properties: [], current: None, map: None},
  reducer: (action, state) =>
    switch (action) {
    | SetMap(map) => ReasonReact.Update({...state, map: Some(map)})
    | SelectProperty(prop) => ReasonReact.Update({...state, current: Some(prop)})
    | UnselectProperty => ReasonReact.Update({...state, current: None})
    | AddProperty(prop) => ReasonReact.UpdateWithSideEffects({...state, properties: [prop, ...state.properties]}, self =>
      switch (self.state.map) {
      | None => ()
      | Some(map) => map |. Leaflet.Map.addGeoJSON(prop.layer) |. Leaflet.Map.fitBounds(Leaflet.GeoJSON.getBounds(prop.layer))|. ignore
      })
    | RemoveProperty(prop) => ReasonReact.UpdateWithSideEffects({...state, properties: List.keep(state.properties, p => p!=prop)}, self => 
      switch (self.state.map) {
      | None => ()
      | Some(map) => map |. Leaflet.Map.removeGeoJSON(prop.layer) |. ignore
      })
    },
  render: self =>
    <div className="App">
      <MaterialUI.CssBaseline />
      <MaterialUI.AppBar position=`Static color=`Inherit>
        <MaterialUI.Toolbar>
          <MaterialUI.Typography variant=`Title color=`Inherit>
            (s("Terra Claims"))
          </MaterialUI.Typography>
        </MaterialUI.Toolbar>
      </MaterialUI.AppBar>
      <MapView setMap=((map) => self.send(SetMap(map))) />
    </div>,
};