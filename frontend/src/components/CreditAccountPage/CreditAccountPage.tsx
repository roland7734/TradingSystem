import React, { useState } from "react";
import "./CreditAccountPage.scss";
import { useAppMode } from "../../context/AppModeContext";
import { creditAccount } from "../../api/accounts";

const CreditAccountForm: React.FC = () => {
  const [userId, setUserId] = useState("");
  const [amount, setAmount] = useState<number>(0);
  const [message, setMessage] = useState<string | null>(null);
  const { mode } = useAppMode();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (mode === "REWIND") {
      setMessage("Cannot credit account in REWIND mode.");
      return;
    }
    try {
      const res = await creditAccount({ user_id: userId, amount });
      setMessage(`Success: ${res.message}`);
    } catch (err: any) {
      setMessage("Failed to credit account");
    }
  };

  return (
    <div className="credit-form-container">
      <h2>Credit Account</h2>
      <form onSubmit={handleSubmit}>
        <label>User ID</label>
        <input
          type="text"
          value={userId}
          onChange={(e) => setUserId(e.target.value)}
          required
          disabled={mode === "REWIND"}
        />

        <label>Amount</label>
        <input
          type="number"
          value={amount}
          onChange={(e) => setAmount(Number(e.target.value))}
          required
          disabled={mode === "REWIND"}
        />

        <button type="submit" disabled={mode === "REWIND"}>
          Credit
        </button>
      </form>
      {message && <p className="feedback">{message}</p>}
    </div>
  );
};

export default CreditAccountForm;
