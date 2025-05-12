export interface OrderRequest {
  user_id: string;
  type: "BUY" | "SELL";
  quantity: number;
  price: number;
}

export interface CancelOrderParams {
  orderId: string;
  userId: string;
}

export interface Order {
  order_id: string;
  user_id: string;
  type: "BUY" | "SELL";
  quantity: number;
  price: number;
  timestamp: string;
}
