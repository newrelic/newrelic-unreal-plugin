import { compress } from "https://deno.land/x/zip@v1.2.5/mod.ts";

const supportedVersions = ['5.0', '5.1', '5.2', '5.3'];

const pluginJson = JSON.parse(await Deno.readTextFile('./Plugins/NewRelic/NewRelic.uplugin'));

for (const unrealVersion of supportedVersions) {
  const pluginVersion = pluginJson.VersionName;
  pluginJson.EngineVersion = `${unrealVersion}.0`;
  await Deno.writeTextFile('./Plugins/NewRelic/NewRelic.uplugin', JSON.stringify(pluginJson, null, 4));
  
  const zipName = `newrelic-unreal-plugin-${pluginVersion}-unreal-${unrealVersion}.zip`;
  const zipEntries = [
    'Plugins/NewRelic/Config',
    'Plugins/NewRelic/Resources',
    'Plugins/NewRelic/Source',
    'Plugins/NewRelic/NewRelic.uplugin',
    'README.md',
  ];
  
  console.log(`About to create release package for Unreal Engine ${unrealVersion}...`);
  
  await compress(zipEntries, zipName);
  
  console.log(`Created ${zipName} successfully!`);
}
