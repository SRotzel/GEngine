using System.Text.Json;

namespace GBuildSystem 
{
    public sealed class GModuleRules
    {
        public string ModuleName { get; set; }
        public List<string> PublicDependencies { get; set; }
        public List<string> PrivateDependencies { get; set; }

        public GModuleRules(string moduleName)
        {
            ModuleName = moduleName;
            PublicDependencies = new List<string>();
            PrivateDependencies = new List<string>();
        }
    }

    class GModule 
    {
        public static void CreateModule(string moduleName, string dir)
        {
            Console.WriteLine("    Creating module: " + moduleName);
            string modDir = Path.Combine(dir, moduleName);

            Directory.CreateDirectory(modDir);
            Directory.CreateDirectory(Path.Combine(modDir, "Private"));
            Directory.CreateDirectory(Path.Combine(modDir, "Public"));

            string modHeader = Path.Combine(modDir, "Public", moduleName + ".h");
            string srcFile   = Path.Combine(modDir, "Private", moduleName + ".cpp");

            File.WriteAllText(
                modHeader,
                GTemplateCode.GModuleHeaderTemplate(moduleName)
            );
            File.WriteAllText(
                srcFile,
                GTemplateCode.GModuleSourceTemplate(moduleName)
            );
            

            var moduleRules = new GModuleRules(moduleName);
            string jsonString = JsonSerializer.Serialize(moduleRules, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(Path.Combine(modDir, moduleName + ".GModule"), jsonString);
        }
    }
}