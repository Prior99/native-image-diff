import * as path from "path";
import * as fs from "fs";
import * as http from "http";
import { qualifiedName } from "../scripts/file-name";

// istanbul ignore if
if (!fs.existsSync(qualifiedName)) {
    throw new Error(`Unable to find native addon file "${qualifiedName}".`);
}

export const {
    __native_imageDiff,
} = require(qualifiedName); // tslint:disable-line
