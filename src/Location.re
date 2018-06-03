[@bs.deriving abstract]
type coords = {
  latitude: float,
  longitude: float
};

[@bs.deriving abstract]
type position = {
  timestamp: Js.Date.t,
  coords: coords
};

[@bs.deriving accessors]
type errorType = 
  | [@bs.as 1] PermissionDenied
  | PositionUnvailable
  | RequestTimeout;

[@bs.deriving abstract]
type positionError = {
  [@bs.as "error"] errorType: errorType,
  message: string
};

[@bs.deriving abstract]
type positionOptions = {
  [@bs.optional] enableHighAccuracy: bool,
  [@bs.optional] timeout: int,
  [@bs.optional] maximumAge: int,
};

[@bs.val] [@bs.scope ("navigator", "geolocation")] external rawGetCurrentPosition : (position => unit, positionError => unit)=> unit = "getCurrentPosition";


let getCurrentPosition = (onSuccess, onError) =>
  rawGetCurrentPosition(position => onSuccess(coords(position)), error => onError(errorType(error)));
