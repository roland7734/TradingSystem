import React from "react";
import { BrowserRouter, Routes, Route } from "react-router-dom";
import Header from "./components/Header/Header";
import WarningBanner from "./components/Shared/WarningBanner";
import Home from "./components/HomePage/HomePage";
import CreditAccountPage from "./components/CreditAccountPage/CreditAccountPage";
import PlaceOrderPage from "./components/PlaceOrderPage/PlaceOrderPage";
import OrderHistoryPage from "./components/OrderHistoryPage/OrderHistoryPage";
import ActiveOrdersPage from "./components/ActiveOrdersPage/ActiveOrdersPage";
import AdminPage from "./components/AdminPage/AdminPage";
import BalancePage from "./components/BalancePage/BalancePage";

const App: React.FC = () => {
  return (
    <BrowserRouter>
      <Header />
      <WarningBanner />
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/credit" element={<CreditAccountPage />} />
        <Route path="/trade" element={<PlaceOrderPage />} />
        <Route path="/orders" element={<ActiveOrdersPage />} />
        <Route path="/history" element={<OrderHistoryPage />} />
        <Route path="/balance" element={<BalancePage />} />
        <Route path="/admin" element={<AdminPage />} />
      </Routes>
    </BrowserRouter>
  );
};

export default App;
