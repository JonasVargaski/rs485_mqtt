import { MqttDevice } from "./device-view";
import { getDevice } from "@/services/devices";
import { redirect } from "next/navigation";

type Props = {
  params: { id?: string };
};

export default async function Home({ params }: Props) {
  if (!params.id) return redirect("/device/not-found");
  const [device] = await getDevice(params.id);
  if (!device) return redirect("/device/not-found");
  if (!device.enabled) return redirect("/device/authorize");

  return (
    <div className="flex items-center min-w-max sm:min-w-0 justify-center relative w-full overflow-x-auto px-2 no-scrollbar">
      <MqttDevice id={device.device_id} name={device.device_name} />
    </div>
  );
}
