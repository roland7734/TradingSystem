import React from "react";
import { useAppMode } from "../../context/AppModeContext";
import "./WarningBanner.scss";

const WarningBanner: React.FC = () => {
  const { mode } = useAppMode();

  if (mode !== "REWIND") return null;

  return (
    <div className="warning-banner">
      The application is currently in <strong>REWIND</strong> mode. State is
      frozen and read-only.
    </div>
  );
};

export default WarningBanner;
