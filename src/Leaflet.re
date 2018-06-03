module CRS = {
  type t;
};

module Layer = {
  type t;
};

type zoom = int;
type coordinate = (float, float);

module Point = {
  [@bs.deriving abstract]
  type t = {
    x: int,
    y: int
  };
  [@bs.send] external add : (t, t) => t = "";
  [@bs.send] external subtract : (t, t) => t = "";
  [@bs.send] external divideBy : (t, float) => t = "";
  [@bs.send] external multiplyBy : (t, float) => t = "";
  [@bs.send] external scaleBy : (t, float) => t = "";
  [@bs.send] external unscaleBy : (t, float) => t = "";
  [@bs.send] external distanceTo : (t, t) => float = "";
  [@bs.send] external contains : (t, t) => bool = "";
  [@bs.send] external equals : (t, t) => bool = "";
  [@bs.get] external x : t => float = "";
  [@bs.get] external y : t => float = "";
};

module LatLngBounds = {
  type t;
};

module LatLng = {
  [@bs.deriving abstract]
  type t = {
    lat: float,
    lng: float,
    [@bs.optional] alt: float
  };

  [@bs.send] external toString : t => string = "";
  [@bs.send] external equals : (t, t) => bool = "";
  [@bs.send] external distanceTo : (t, t) => float = "";
  [@bs.send] external wrap : t => t = "";
  [@bs.send] external toBounds : (t, int) => LatLngBounds.t = "";
};

module GeoJSON = {
  type t;
  [@bs.send] external getBounds : t => LatLngBounds.t = "";
};


module Map = {
  type t;
  /* Interaction Options */
  /* Map State Options */
  [@bs.send] external crs : t => CRS.t = "";
  [@bs.send] [@bs.return undefined_to_opt]
  external center : t => option(LatLng.t) = "getCenter";
  [@bs.send] [@bs.return undefined_to_opt]
  external zoom : t => option(int) = "getZoom";
  [@bs.send] [@bs.return undefined_to_opt]
  external maxZoom : t => option(int) = "";
  [@bs.send] [@bs.return undefined_to_opt]
  external minZoom : t => option(int) = "";
  [@bs.send] external layers : t => list(Layer.t) = "";
  [@bs.send] [@bs.return null_to_opt]
  external maxBounds : t => option(LatLngBounds.t) = "";
  /* Methods for Layers and Controls */
  [@bs.send] external addLayer : (t, Layer.t) => t = "";
  [@bs.send] external addGeoJSON : (t, GeoJSON.t) => t = "addLayer";
  [@bs.send] external removeLayer : (t, Layer.t) => t = "";
  [@bs.send] external removeGeoJSON : (t, GeoJSON.t) => t = "removeLayer";
  [@bs.send] external hasLayer : (t, Layer.t) => bool = "";
  /* Methods for Navigation */
  [@bs.send] external setView : (t, (float, float), int) => t = "";
  [@bs.send] external setZoom : (t, int) => t = "";
  [@bs.send] external fitBounds : (t, LatLngBounds.t) => t = "";
  
  /* Events */
  [@bs.send] external on : (t, string, unit => unit) => unit = "";
  [@bs.send] external off : (t, string, unit => unit) => unit = "";
};

module Control = {
  type t;
};


[@bs.deriving abstract]
type map_options = {
  zoom: zoom,
  center: coordinate
};

[@bs.deriving abstract]
type tile_layer_options = {
  [@bs.optional] minZoom: zoom,
  [@bs.optional] maxZoom: zoom,
  attribution: string
};

[@bs.module "leaflet"]
external tileLayer : (string, tile_layer_options) => Layer.t = "";

[@bs.module "leaflet"]
external latLngBounds : (LatLng.t, LatLng.t) => LatLngBounds.t = "";

[@bs.module "leaflet"] external latLng : ((float, float)) => LatLng.t = "";

[@bs.module "leaflet"]
external latLngAlt : ((float, float, float)) => LatLng.t = "latLng";

let create_lat_lng = (~latitude, ~longitude, ~altitude) =>
  switch (altitude) {
  | Some(alt) => latLngAlt((latitude, longitude, alt))
  | _ => latLng((latitude, longitude))
  };

[@bs.module "leaflet"] external point_int : (int, int) => Point.t = "point";

[@bs.module "leaflet"]
external point_float : (float, float, bool) => Point.t = "point";

[@bs.module "leaflet"]
external create_map : (string) => Map.t = "map";

[@bs.module "leaflet"]
external create_geo_json : (Js.Json.t) => GeoJSON.t = "geoJSON";
