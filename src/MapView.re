[%bs.raw {|require('./MapView.css')|}];

type state = {
  zoom: int,
  center: (float, float),
  map: option(Leaflet.Map.t),
};

type action =
  | SetMap(Leaflet.Map.t)
  | Move(float, float)
  | UpdateCenter(float, float)
  | Zoom(int)
  | UpdateZoom(int);

let component = ReasonReact.reducerComponent("Map");

let osmUrl = "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png";

let osmAttrib = {|Map data © <a href="https://openstreetmap.org">OpenStreetMap</a> contributors|};

let osm =
  Leaflet.tileLayer(
    osmUrl,
    Leaflet.tile_layer_options(~attribution=osmAttrib, ()),
  );

let london = (51.505, (-0.09));

let managua = (12.0495, (-86.2657));

/* let map_options = myState => LeafletRe.map_options(center=myState.center, zoom=myState.zoom); */
let moveTo = ({map, center, zoom}) =>
  switch (map) {
  | None => ()
  | Some(map) => map |. Leaflet.Map.setView(center, zoom) |. ignore
  };

let zoomTo = ({map, zoom}) =>
  switch (map) {
  | None => ()
  | Some(map) => map |. Leaflet.Map.setZoom(zoom) |. ignore
  };

let make = (~setMap, _children) => {
  ...component,
  initialState: () => {zoom: 15, center: managua, map: None},
  reducer: (action, state) =>
    switch (action) {
    | SetMap(map) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, map: Some(map)},
        (
          self =>
            map
            |. Leaflet.Map.setView(self.state.center, self.state.zoom)
            |. Leaflet.Map.addLayer(osm)
            |. setMap
            |. ignore
        ),
      )
    | Move(lat, lon) =>
      ReasonReact.SideEffects(
        (self => moveTo({...self.state, center: (lat, lon)})),
      )
    | UpdateCenter(lat, lon) =>
      ReasonReact.Update({...state, center: (lat, lon)})
    | Zoom(zoom) =>
      ReasonReact.SideEffects((self => zoomTo({...self.state, zoom})))
    | UpdateZoom(zoom) => ReasonReact.Update({...state, zoom})
    },
  didMount: self => {
    /* Location.getCurrentPosition(coords => self.send(Move(Location.latitude(coords), Location.longitude(coords))), error => Js.log(error)); */
    let map = Leaflet.create_map("map");
    self.send(SetMap(map));
    let updateMove = () => {
      let center = Leaflet.Map.center(map);
      switch (center) {
      | None => ()
      | Some(pos) =>
        self.send(
          UpdateCenter(Leaflet.LatLng.lat(pos), Leaflet.LatLng.lng(pos)),
        );
        ();
      };
    };
    let updateZoom = () => {
      let level = Leaflet.Map.zoom(map);
      switch (level) {
      | None => ()
      | Some(level) =>
        self.send(UpdateZoom(level));
        ();
      };
    };
    Leaflet.Map.on(map, "move", updateMove);
    self.onUnmount(() => Leaflet.Map.off(map, "move", updateMove));
    Leaflet.Map.on(map, "moveend", updateMove);
    self.onUnmount(() => Leaflet.Map.off(map, "moveend", updateMove));
    Leaflet.Map.on(map, "zoom", updateZoom);
    self.onUnmount(() => Leaflet.Map.off(map, "zoom", updateZoom));
    Leaflet.Map.on(map, "zoomend", updateZoom);
    self.onUnmount(() => Leaflet.Map.off(map, "zoomend", updateZoom));
  },
  render: _self => <div id="map" className="Map" />,
};