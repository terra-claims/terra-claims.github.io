[%bs.raw {|require('./App.css')|}];


let component = ReasonReact.statelessComponent("App");

let s = ReasonReact.string;

let make = (_children) => {
  ...component,
  render: _self =>
    <div className="App">
      <MaterialUI.CssBaseline />
      <MaterialUI.AppBar position=`Static color=`Inherit>
        <MaterialUI.Toolbar>
          <MaterialUI.Typography variant=`Title color=`Inherit>
            (s("Terra Claims"))
          </MaterialUI.Typography>
        </MaterialUI.Toolbar>
      </MaterialUI.AppBar>
      <Map />
    </div>,
};