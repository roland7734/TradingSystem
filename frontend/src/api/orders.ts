import type {
  OrderRequest,
  CancelOrderParams,
  Order,
} from "../models/OrderModels";
import axiosInstance from "./axiosInstance";

export const placeOrder = async (order: OrderRequest) => {
  const response = await axiosInstance.post("/orders", order);
  return response.data;
};

export const cancelOrder = async (params: CancelOrderParams) => {
  const { orderId, userId } = params;
  const response = await axiosInstance.delete(`/orders/${orderId}`, {
    params: { user_id: userId },
  });
  return response.data;
};

export const getOrderBook = async (): Promise<Order[]> => {
  const response = await axiosInstance.get("/orderbook");
  return response.data;
};

export const getOrderBookAtTimestamp = async (
  timestamp: string
): Promise<Order[]> => {
  const encodedTs = encodeURIComponent(timestamp);
  const response = await axiosInstance.get(`/orderbook/timestamp/${encodedTs}`);
  return response.data;
};
