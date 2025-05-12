import React, { createContext, useContext, useState } from "react";
import type { ReactNode } from "react";

type AppMode = "LIVE" | "REWIND";

interface AppModeContextType {
  mode: AppMode;
  setMode: (mode: AppMode) => void;
  timestamp: string | null;
  setTimestamp: (ts: string | null) => void;
}

const AppModeContext = createContext<AppModeContextType | undefined>(undefined);

export const AppModeProvider: React.FC<{ children: ReactNode }> = ({
  children,
}) => {
  const [mode, setMode] = useState<AppMode>("LIVE");
  const [timestamp, setTimestamp] = useState<string | null>(null);

  return (
    <AppModeContext.Provider value={{ mode, setMode, timestamp, setTimestamp }}>
      {children}
    </AppModeContext.Provider>
  );
};

export const useAppMode = (): AppModeContextType => {
  const context = useContext(AppModeContext);
  if (!context) {
    throw new Error("useAppMode must be used within an AppModeProvider");
  }
  return context;
};
