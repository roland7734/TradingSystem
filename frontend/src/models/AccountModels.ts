export interface CreditAccountRequest {
  user_id: string;
  amount: number;
}

export interface AccountBalance {
  user_id: string;
  cash_balance: number;
}
