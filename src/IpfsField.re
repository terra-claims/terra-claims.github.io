[%bs.raw {|require('./IpfsField.css')|}];
type status =
  | Fetching
  | Empty
  | InvalidEntry
  | ValidEntry
  | Success
  | Error;

type state = {
  ipfsHash: option(string),
  status,
  message: option(string)
};

type action =
  | Clear
  | Update(string)
  | Submit
  | Succeed
  | Fail(string);

let component = ReasonReact.reducerComponent("IpfsField");

let s = ReasonReact.string;

let ipfsHashString = (hash) => {
  switch (hash) {
  | None => ""
  | Some(hash) => hash
  };
};

let ipfsMatcher = [%bs.re "/^[Q][1-9A-HJ-NP-Za-km-z]{44,46}$/"];

let checkIpfsHash = (hash) =>
  switch (hash) {
  | "" => Empty
  | hash =>
    if (Js.Re.test(hash, ipfsMatcher)) {
      ValidEntry;
    } else {
      InvalidEntry;
    }
  };

let validEntry = (status) => {
  switch (status) {
  | ValidEntry => true
  | _ => false
  };
};

let make = (_children) => {
  ...component,
  initialState: () => {ipfsHash: None, status: Empty, message: None},
  reducer: (action, state) => {
    switch (action) {
    | Clear => ReasonReact.Update({ipfsHash: None, status: Empty, message: None})
    | Update(address) => ReasonReact.Update({...state, status: checkIpfsHash(address), ipfsHash: Some(address)})
    | Submit => ReasonReact.Update({...state, status: Fetching})
    | Succeed => ReasonReact.Update({...state, status: Success})
    | Fail(message) => ReasonReact.Update({...state, status: Error, message: Some(message)})
    }
  },
  render: self =>
    <div className="IpfsField">
      <MaterialUI.Typography variant=`Subheading color=`Inherit align=`Center>
        <MaterialUI.TextField 
          placeholder="IPFS hash of GeoJSON"
          value=(`String(ipfsHashString(self.state.ipfsHash)))
          onChange=(
            event => 
              self.send(
                Update(
                  ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value
                ),
              )
          )
        />
        <MaterialUI.IconButton onClick=(_ => validEntry(self.state.status) ? self.send(Submit) : ())>
          <MaterialUI.Icon color=(validEntry(self.state.status) ? `Primary : `Disabled)>(s("search"))</MaterialUI.Icon>
        </MaterialUI.IconButton>
      </MaterialUI.Typography>
    </div>,
};