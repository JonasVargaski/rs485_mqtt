import { Nav } from "@/components/nav";
import "./globals.css";
import type { Metadata } from "next";
import { Inter } from "next/font/google";

const inter = Inter({ subsets: ["latin"] });

export const metadata: Metadata = {
  title: "App - monitor",
  description: "",
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body className={inter.className}>
        <div className="relative flex min-h-screen flex-col">
          <header className="supports-backdrop-blur:bg-background/60 sticky top-0 z-40 w-full border-b border-slate-900 bg-background/95 backdrop-blur">
            <div className="container flex h-11 items-center">
              <Nav />
            </div>
          </header>
          {children}
        </div>
      </body>
    </html>
  );
}
