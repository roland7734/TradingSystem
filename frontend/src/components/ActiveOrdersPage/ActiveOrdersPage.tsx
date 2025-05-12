import React, { useEffect, useState } from "react";
import "./ActiveOrdersPage.scss";
import { useAppMode } from "../../context/AppModeContext";
import axiosInstance from "../../api/axiosInstance";
import { getOrderBook, getOrderBookAtTimestamp } from "../../api/orders";
import type { Order } from "../../models/OrderModels";

const ActiveOrdersPage: React.FC = () => {
  const [orders, setOrders] = useState<Order[]>([]);
  const [message, setMessage] = useState<string | null>(null);
  const { mode, timestamp } = useAppMode();

  const fetchOrders = async () => {
    try {
      const data =
        mode === "REWIND" && timestamp
          ? await getOrderBookAtTimestamp(timestamp)
          : await getOrderBook();
      setOrders(data);
    } catch (err) {
      setMessage("Failed to fetch active orders");
    }
  };

  const cancelOrder = async (orderId: string, userId: string) => {
    try {
      await axiosInstance.delete(`/orders/${orderId}?user_id=${userId}`);
      setMessage("Order cancelled successfully");
      fetchOrders();
    } catch (err) {
      setMessage("Failed to cancel order");
    }
  };

  useEffect(() => {
    fetchOrders();
  }, [mode, timestamp]);

  return (
    <div className="active-orders-container">
      <h2>Active Orders</h2>
      {message && <p className="status">{message}</p>}
      <table>
        <thead>
          <tr>
            <th>Order ID</th>
            <th>Timestamp</th>
            <th>User</th>
            <th>Type</th>
            <th>Quantity</th>
            <th>Price</th>
            {mode === "LIVE" && <th>Action</th>}
          </tr>
        </thead>
        <tbody>
          {orders.map((order) => (
            <tr key={order.order_id}>
              <td>{order.order_id}</td>
              <td>{order.timestamp}</td>
              <td>{order.user_id}</td>
              <td>{order.type}</td>
              <td>{order.quantity}</td>
              <td>{order.price}</td>
              {mode === "LIVE" && (
                <td>
                  <button
                    className="cancel-button"
                    onClick={() => cancelOrder(order.order_id, order.user_id)}
                  >
                    Cancel
                  </button>
                </td>
              )}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default ActiveOrdersPage;
