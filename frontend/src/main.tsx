import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import { AppModeProvider } from "./context/AppModeContext";

const root = ReactDOM.createRoot(document.getElementById("root")!);

root.render(
  <React.StrictMode>
    <AppModeProvider>
      <App />
    </AppModeProvider>
  </React.StrictMode>
);
