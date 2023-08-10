import { ReactNode } from "react";

type TDeviceLayoutProps = {
  children: ReactNode;
};

export default function DeviceLayout({ children }: TDeviceLayoutProps) {
  return (
    <div className="container flex flex-col items-center justify-center h-[50vh]">
      {children}
    </div>
  );
}
