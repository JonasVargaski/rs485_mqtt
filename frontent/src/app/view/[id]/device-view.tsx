"use client";

import { Device } from "@/components/device";
import { useMqttDevice } from "@/hooks/useMqttDevice";

type TMqttDeviceProps = {
  id: string;
  name: string;
};

export function MqttDevice({ id, name }: TMqttDeviceProps) {
  const parsedDevices = useMqttDevice({
    devices: [{ id, name }],
    diffInSeconds: 10,
    intervalCheckInSeconds: 5,
  });

  return parsedDevices.map((device) => (
    <Device key={device.id} data={device} />
  ));
}
