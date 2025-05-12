import React from "react";
import "./HomePage.scss";
import { useNavigate } from "react-router-dom";

const Home: React.FC = () => {
  const navigate = useNavigate();

  return (
    <div className="home-page">
      <h1>Welcome to Event Trader</h1>
      <p>
        This is a fully event-sourced trading system. All state (balances,
        orders, trades) is reconstructed from a historical log of domain events.
        You can trade, audit, or rewind time.
      </p>

      <div className="quick-actions">
        <button onClick={() => navigate("/credit")}>Fund User</button>
        <button onClick={() => navigate("/trade")}>Place Order</button>
        <button onClick={() => navigate("/orders")}>View Orders</button>
        <button onClick={() => navigate("/admin")}>Admin Tools</button>
      </div>
    </div>
  );
};

export default Home;
