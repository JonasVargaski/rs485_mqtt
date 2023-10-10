import { Nav } from "@/components/nav";
import "./globals.css";
import type { Metadata } from "next";
import { Inter } from "next/font/google";
import Script from "next/script";

const inter = Inter({ subsets: ["latin"] });

export const metadata: Metadata = {
  title: "App - monitor",
  description: "",
};

const GTM_ID = "GTM-W69NKT5S";

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <Script id="google-tag-manager" strategy="afterInteractive">
        {`
        (function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
        new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
        j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
        'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
        })(window,document,'script','dataLayer','${GTM_ID}');
        `}
      </Script>

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
