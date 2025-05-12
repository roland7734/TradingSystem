import React, { useState } from "react";
import "./BalancePage.scss";
import { useAppMode } from "../../context/AppModeContext";
import {
  getAccountBalanceAtTimestamp,
  getAccountBalance,
} from "../../api/accounts";

const BalancePage: React.FC = () => {
  const [userId, setUserId] = useState("");
  const [cash, setCash] = useState<number | null>(null);
  const [error, setError] = useState("");
  const { mode, timestamp } = useAppMode();

  const fetchBalance = async () => {
    try {
      const data =
        mode === "REWIND" && timestamp
          ? await getAccountBalanceAtTimestamp(userId, timestamp)
          : await getAccountBalance(userId);
      setCash(data.cash_balance);
      setError("");
    } catch (e) {
      setError("User not found or error fetching balance");
      setCash(null);
    }
  };

  return (
    <div className="balance-page">
      <h2>Check User Balance</h2>
      <div className="form-group">
        <input
          type="text"
          placeholder="User ID"
          value={userId}
          onChange={(e) => setUserId(e.target.value)}
        />
        <button onClick={fetchBalance}>Check Balance</button>
      </div>
      {error && <p className="error">{error}</p>}
      {cash !== null && (
        <div className="result">
          <p>Cash Balance: ${cash}</p>
        </div>
      )}
    </div>
  );
};

export default BalancePage;
