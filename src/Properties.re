
type property = {
  ipfsHash: string,
  geoJSON: Js.Json.t,
  layer: Leaflet.GeoJSON.t,
};

type properties = list(property);


