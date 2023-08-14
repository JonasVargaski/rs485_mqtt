"use client";

import { Device } from "@/components/device";
import { TDevice, useMqttDevice } from "@/hooks/useMqttDevice";

type TViewAllProps = {
  devices: TDevice[];
};

export function ViewAll({ devices }: TViewAllProps) {
  const data = useMqttDevice({
    devices,
    diffInSeconds: 10,
    intervalCheckInSeconds: 5,
  });

  return data.map((device) => <Device key={device.id} data={device} />);
}
