import axiosInstance from "./axiosInstance";

export const replayAllEvents = async () => {
  const response = await axiosInstance.post("/replay/all");
  return response.data;
};

export const replayEventsUntil = async (timestamp: string) => {
  const response = await axiosInstance.post("/replay/until", { timestamp });
  return response.data;
};

export const getAllEvents = async (): Promise<string[]> => {
  const response = await axiosInstance.get("/events");
  return response.data;
};

export const getRawLogFile = async (): Promise<string> => {
  const response = await axiosInstance.get("/events/log", {
    responseType: "text",
  });
  return response.data;
};

export const getEventsUntilTimestamp = async (
  timestamp: string
): Promise<string[]> => {
  const encodedTs = encodeURIComponent(timestamp);
  const response = await axiosInstance.get(`/events/timestamp/${encodedTs}`);
  return response.data;
};
