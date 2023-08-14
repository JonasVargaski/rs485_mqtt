import { getAllDevices } from "@/services/devices";
import { ViewAll } from "./view-all";

export default async function admin() {
  const devices = await getAllDevices();

  return (
    <div className="flex items-center justify-center flex-wrap gap-8 p-2">
      <ViewAll
        devices={devices.map((x) => ({
          ...x,
          id: x.device_id,
          name: x.device_name,
        }))}
      />
    </div>
  );
}
