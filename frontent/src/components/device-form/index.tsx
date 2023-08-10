"use client";

import { useRouter } from "next/navigation";
import { useCallback } from "react";

export function DeviceForm() {
  const router = useRouter();

  const handleSubmit = useCallback(
    async (e: React.FormEvent<HTMLFormElement>) => {
      e.stopPropagation();
      e.preventDefault();

      const device_id = new FormData(e.currentTarget)
        .get("device_id")
        ?.toString()
        .toUpperCase();

      await router.push(`/view/${device_id}`);
    },
    [router]
  );

  return (
    <form
      onSubmit={handleSubmit}
      className="p-6 border border-slate-600 rounded-lg w-full max-w-md"
    >
      <span className="text-center block font-medium">
        Informe o código do dispositivo
      </span>

      <div className="md:flex md:items-center my-6">
        <input
          className="bg-gray-200 appearance-none border-2 border-gray-200 rounded w-full py-3 px-4 text-gray-700 leading-tight focus:outline-none focus:bg-white focus:border-cyan-500"
          pattern="^[0-9a-fA-F]{8}$"
          id="device_id"
          name="device_id"
          type="text"
          required
          placeholder="Ex: 683CD2B1"
        />
      </div>

      <button
        className="shadow font-semibold w-full bg-cyan-700 hover:bg-cyan-800 focus:shadow-outline focus:outline-none text-white py-3 px-4 rounded"
        type="submit"
      >
        Visualizar
      </button>
    </form>
  );
}
