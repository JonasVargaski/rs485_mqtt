import Image from "next/image";
import Marquee from "react-fast-marquee";

type TAlarmIndicatorProps = {
  alarms: string[];
};

export function AlarmIndicator({ alarms }: TAlarmIndicatorProps) {
  return (
    <>
      {alarms.length > 0 && (
        <>
          <Image src="/warning.svg" alt="icon" width={18} height={18} />
          <Marquee
            speed={80}
            className="text-base font-semibold text-yellow-500 truncate uppercase w-full"
          >
            {alarms.join(" | ")}
          </Marquee>
        </>
      )}
    </>
  );
}
