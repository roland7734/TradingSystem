import React from "react";
import { Link, useLocation } from "react-router-dom";
import "./Header.scss";
import { useAppMode } from "../../context/AppModeContext";

const Header: React.FC = () => {
  const location = useLocation();
  const { mode, timestamp } = useAppMode();

  const isActive = (path: string) => location.pathname === path;

  return (
    <header className="main-header">
      <div className="logo">Event Trader</div>
      <nav className="nav-links">
        <Link className={isActive("/") ? "active" : ""} to="/">
          Home
        </Link>
        <Link className={isActive("/credit") ? "active" : ""} to="/credit">
          Fund
        </Link>
        <Link className={isActive("/trade") ? "active" : ""} to="/trade">
          Trade
        </Link>
        <Link className={isActive("/orders") ? "active" : ""} to="/orders">
          Orders
        </Link>
        <Link className={isActive("/history") ? "active" : ""} to="/history">
          History
        </Link>
        <Link className={isActive("/balance") ? "active" : ""} to="/balance">
          Balance
        </Link>
        <Link className={isActive("/admin") ? "active" : ""} to="/admin">
          Admin
        </Link>
      </nav>
      <div
        className={`app-mode-badge ${mode === "REWIND" ? "rewind" : "live"}`}
      >
        {mode === "REWIND" ? `REWIND MODE (${timestamp})` : "LIVE"}
      </div>
    </header>
  );
};

export default Header;
