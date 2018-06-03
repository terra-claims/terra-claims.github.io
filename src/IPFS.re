let infuraUrl = "https://ipfs.infura.io/ipfs/";

let toProperty = (hash, json): Properties.property => ({
  ipfsHash: hash,
  geoJSON: json,
  layer: Leaflet.create_geo_json(json)
});


let fetchProperty = ipfsHash => {
  let url = infuraUrl ++ ipfsHash;
  Js.log(url);
  Js.Promise.(
    Fetch.fetch(url) 
    |> then_(Fetch.Response.json) 
    |> then_(json => resolve(toProperty(ipfsHash, json)))
  )
}
