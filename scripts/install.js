const path = require("path");
const fs = require("fs");
const fetch = require("node-fetch");
const fileName = require("./file-name");

if (fs.existsSync(fileName.qualifiedName)) { return; }
if (process.env["NATIVE_IMAGE_DIFF_SKIP_BINARY_DOWNLOAD_FOR_CI"]) { return; }

// Get the version of the library;
const pkg = require(path.resolve(__dirname, "..", "package.json"));
const packageVersion = pkg.version;
const url = `https://github.com/Prior99/native-image-diff/releases/download/${packageVersion}/${fileName.baseName}`;

fetch(url)
    .then(response => {
        if (response.ok) {
            return response.buffer();
        }
        if (response.status === 404) {
            throw new Error(`No supported native-image-diff ${packageVersion} build found for node ${process.version} on ${process.platform} (${process.arch}).`);
        } else {
            throw new Error(`Error downloading binaries for native-image-diff ${packageVersion}. Received status code ${response.statusCode}`);
        }
    })
    .catch(err => {
        throw new Error(err);
    })
    .then(buffer => {
        fs.writeFileSync(fileName.qualifiedName, buffer);
        console.info("Successfully downloaded binaries for native-image-diff.");
    })
