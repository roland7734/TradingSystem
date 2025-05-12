import type {
  CreditAccountRequest,
  AccountBalance,
} from "../models/AccountModels";
import axiosInstance from "./axiosInstance";

export const creditAccount = async (data: CreditAccountRequest) => {
  const response = await axiosInstance.post("/accounts/credit", data);
  return response.data;
};

export const getAccountBalance = async (
  userId: string
): Promise<AccountBalance> => {
  const response = await axiosInstance.get(`/accounts/${userId}`);
  return response.data;
};

export const getAccountBalanceAtTimestamp = async (
  userId: string,
  timestamp: string
): Promise<AccountBalance> => {
  const encodedTs = encodeURIComponent(timestamp);
  const response = await axiosInstance.get(
    `/accounts/${userId}/timestamp/${encodedTs}`
  );
  return response.data;
};
