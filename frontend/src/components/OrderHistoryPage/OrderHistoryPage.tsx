import React, { useEffect, useState } from "react";
import "./OrderHistoryPage.scss";
import { useAppMode } from "../../context/AppModeContext";
import { getAllEvents, getEventsUntilTimestamp } from "../../api/system";

const OrderHistoryPage: React.FC = () => {
  const [events, setEvents] = useState<string[]>([]);
  const [message, setMessage] = useState<string | null>(null);
  const { mode, timestamp } = useAppMode();

  const fetchEvents = async () => {
    try {
      const data =
        mode === "REWIND" && timestamp
          ? await getEventsUntilTimestamp(timestamp)
          : await getAllEvents();
      setEvents(data);
    } catch (err) {
      setMessage("Failed to fetch event history");
    }
  };

  useEffect(() => {
    fetchEvents();
  }, [mode, timestamp]);

  return (
    <div className="history-container">
      <h2>Events History</h2>
      {message && <p className="status">{message}</p>}
      <div className="event-list">
        {events.map((e, index) => (
          <div key={index} className="event-item">
            {e}
          </div>
        ))}
      </div>
    </div>
  );
};

export default OrderHistoryPage;
