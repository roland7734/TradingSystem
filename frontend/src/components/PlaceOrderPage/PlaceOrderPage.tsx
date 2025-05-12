import React, { useState } from "react";
import "./PlaceOrderPage.scss";
import { useAppMode } from "../../context/AppModeContext";
import { placeOrder } from "../../api/orders";

const PlaceOrderPage: React.FC = () => {
  const [userId, setUserId] = useState("");
  const [type, setType] = useState<"BUY" | "SELL">("BUY");
  const [quantity, setQuantity] = useState<number>(1);
  const [price, setPrice] = useState<number>(1);
  const [message, setMessage] = useState<string | null>(null);
  const { mode } = useAppMode();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (mode === "REWIND") {
      setMessage("Cannot place order in REWIND mode.");
      return;
    }
    try {
      const res = await placeOrder({
        user_id: userId,
        type,
        quantity,
        price,
      });
      setMessage(`Success: ${res.message} (Order ID: ${res.order_id})`);
    } catch (err: any) {
      const details = err?.response?.data?.details || "Unknown error";
      setMessage(`Failed to place order: ${details}`);
    }
  };

  return (
    <div className="order-form-container">
      <h2>Place Order</h2>
      <form onSubmit={handleSubmit}>
        <label>User ID</label>
        <input
          type="text"
          value={userId}
          onChange={(e) => setUserId(e.target.value)}
          required
          disabled={mode === "REWIND"}
        />

        <label>Order Type</label>
        <select
          value={type}
          onChange={(e) => setType(e.target.value as "BUY" | "SELL")}
          disabled={mode === "REWIND"}
        >
          <option value="BUY">BUY</option>
          <option value="SELL">SELL</option>
        </select>

        <label>Quantity</label>
        <input
          type="number"
          value={quantity}
          onChange={(e) => setQuantity(Number(e.target.value))}
          min={1}
          required
          disabled={mode === "REWIND"}
        />

        <label>Price</label>
        <input
          type="number"
          value={price}
          onChange={(e) => setPrice(Number(e.target.value))}
          required
          disabled={mode === "REWIND"}
        />

        <button type="submit" disabled={mode === "REWIND"}>
          Submit Order
        </button>
      </form>
      {message && <p className="feedback">{message}</p>}
    </div>
  );
};

export default PlaceOrderPage;
