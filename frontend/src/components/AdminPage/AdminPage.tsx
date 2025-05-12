import React, { useState } from "react";
import { useAppMode } from "../../context/AppModeContext";
import "./AdminPage.scss";
import axiosInstance from "../../api/axiosInstance";

const AdminPage: React.FC = () => {
  const [GUITimestamp, setGUITimestamp] = useState("");
  const [message, setMessage] = useState("");
  const { setMode, setTimestamp } = useAppMode();

  const isValidTimestamp = (input: string): boolean => {
    const regex = /^(\d{2})\/(\d{2})\/(\d{4}) (\d{2}):(\d{2}):(\d{2})$/;
    const match = input.match(regex);

    if (!match) return false;

    const [, dd, mm, yyyy, HH, MM, SS] = match.map(Number);

    if (
      mm < 1 ||
      mm > 12 ||
      dd < 1 ||
      dd > 31 ||
      HH < 0 ||
      HH > 23 ||
      MM < 0 ||
      MM > 59 ||
      SS < 0 ||
      SS > 59
    ) {
      return false;
    }

    const daysInMonth = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    if (dd > daysInMonth[mm - 1]) return false;

    return true;
  };

  const replayAll = async () => {
    try {
      await axiosInstance.post("/replay/all");
      setMessage("Full state replayed successfully.");
      setMode("LIVE");
    } catch (e) {
      setMessage("Failed to replay all events.");
    }
  };

  const replayUntil = async () => {
    if (!isValidTimestamp(GUITimestamp)) {
      setMessage("Invalid timestamp format or value. Use DD/MM/YYYY HH:MM:SS");
      return;
    }

    try {
      await axiosInstance.post("/replay/until", { timestamp: GUITimestamp });
      setMessage(`State replayed up to ${GUITimestamp}`);
      setMode("REWIND");
      setTimestamp(GUITimestamp);
    } catch (e) {
      setMessage("Server error during replay.");
    }
  };

  return (
    <div className="admin-panel">
      <h2>Admin Panel</h2>

      <div className="section">
        <button onClick={replayAll}>Replay All Events</button>
      </div>

      <div className="section">
        <input
          type="text"
          placeholder="DD/MM/YYYY HH:MM:SS"
          value={GUITimestamp}
          onChange={(e) => setGUITimestamp(e.target.value)}
        />
        <button onClick={replayUntil}>Replay Until Timestamp</button>
      </div>

      {message && <p className="message">{message}</p>}
    </div>
  );
};

export default AdminPage;
