"use client";

import { useRouter } from "next/navigation";

export function Nav() {
  const router = useRouter();

  return (
    <div className="flex items-center gap-3">
      <button
        className="text-white font-medium px-3 py-1 rounded-md"
        onClick={() => router.push("/")}
      >
        Início
      </button>
    </div>
  );
}
