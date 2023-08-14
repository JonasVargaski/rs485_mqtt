import { db } from "@/lib/firebase";
import { collection, getDocs, query, where } from "firebase/firestore";

type TDevice = {
  user_id: string;
  device_id: string;
  enabled: boolean;
  device_name: string;
  created_at: Date;
  updated_at: Date;
};

export async function getDevice(device_id: string) {
  const devicesRef = await getDocs(
    query(
      collection(db, "devices").withConverter({
        toFirestore: (data: TDevice) => data,
        fromFirestore: (snap) => {
          const data = snap.data();
          return {
            ...data,
            created_at: data.created_at.toDate(),
            updated_at: data.updated_at.toDate(),
          } as TDevice;
        },
      }),
      where("device_id", "==", device_id)
    )
  );

  return devicesRef.docs.map((x) => ({ ...x.data(), id: x.id }));
}

export async function getAllDevices() {
  const devicesRef = await getDocs(
    query(
      collection(db, "devices").withConverter({
        toFirestore: (data: TDevice) => data,
        fromFirestore: (snap) => {
          const data = snap.data();
          return {
            ...data,
            created_at: data.created_at.toDate(),
            updated_at: data.updated_at.toDate(),
          } as TDevice;
        },
      })
    )
  );

  return devicesRef.docs.map((x) => ({ ...x.data(), id: x.id }));
}
