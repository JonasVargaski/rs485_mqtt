/** @type {import('next').NextConfig} */
const nextConfig = {
  webpack: (config) => {
    Object.assign(config.externals, {
      bufferutil: "bufferutil",
      "utf-8-validate": "utf-8-validate",
    });

    return config;
  },
};

module.exports = nextConfig;
