import childProcess from 'child_process';
import { promises as fsp } from 'fs';

const ZML_VERSION = '7.1.2';

const exec = (command) => new Promise((resolve, reject) => {
    const child = childProcess.exec(command, undefined, err => err ? reject(err) : resolve());
    child.stdout?.pipe(process.stdout);
    child.stderr?.pipe(process.stderr);
  });

export const update_support_package = async () => {
    const fromFolder = `downloads/ZML/CPP/${ZML_VERSION}`;
    const localDestination = `zaberMotionSupport/ZaberMotionCppSupport-${ZML_VERSION}.zip`;

    await fsp.rm('zaberMotionSupport/ZaberMotionCppSupport', { recursive: true }).catch(() => false);
    await exec(`aws s3 cp ${process.env.BUCKET}/${fromFolder}/ZaberMotionCppSupport-${ZML_VERSION}.zip ${localDestination}`);
    await exec(`unzip ${localDestination} -d zaberMotionSupport`);
    await fsp.rm(localDestination);
}

