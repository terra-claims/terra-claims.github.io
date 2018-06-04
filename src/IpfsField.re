[%bs.raw {|require('./IpfsField.css')|}];
open Belt;

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

/*
Js.Promise.(
        IPFS.get(hash) |> ignore
        /* |> then_(property: App.property => {
          /* Js.log(property); */
          addProperty(property);
          self.send(Succeed);
          resolve(());
        }) */
        /* |> Js.Promise.catch(error => {
          Js.log(error);
          self.send(Fail(error.message));
          Js.Promise.resolve();
        });   */

      )

*/

let make = (~addProperty, ~selectProperty, ~properties, _children) => {
  ...component,
  initialState: () => {ipfsHash: None, status: Empty, message: None},
  reducer: (action, state) => {
    switch (action) {
    | Clear => ReasonReact.Update({ipfsHash: None, status: Empty, message: None})
    | Update(address) => ReasonReact.Update({...state, status: checkIpfsHash(address), ipfsHash: Some(address)})
    | Submit => ReasonReact.UpdateWithSideEffects({...state, status: Fetching}, self => {
      switch(self.state.ipfsHash) {
      | None => ()
      | Some(hash) => {
        if (Map.String.has(properties, hash)) {
          selectProperty(hash);
        } else {
          IPFS.fetchProperty(hash) 
          |> Js.Promise.then_(prop => {
            addProperty(prop);
            self.send(Succeed);
            Js.Promise.resolve();
          }) 
          |> Js.Promise.catch(error => {
            Js.log(error);
            /* self.send(Fail(error.message)); */
            Js.Promise.resolve();
          })
          |> ignore  
        }
      }
    }})
    | Succeed => ReasonReact.Update({...state, status: Success})
    | Fail(message) => ReasonReact.Update({...state, status: Error, message: Some(message)})
    }
  },
  render: self =>
    <div className="IpfsField">
      <MaterialUI.TextField
        className="Input"
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
      <MaterialUI.IconButton 
        className="SearchButton"
        onClick=(_ => validEntry(self.state.status) ? self.send(Submit) : ())>
        <MaterialUI.Icon color=(validEntry(self.state.status) ? `Primary : `Disabled)>(s("search"))</MaterialUI.Icon>
      </MaterialUI.IconButton>
    </div>,
};