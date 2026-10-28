using System.Text.Json;
using System.Security.Cryptography;
using System.Security.AccessControl;

namespace GBuildSystem 
{
    public sealed class GModuleRules
    {
        public string ModuleName { get; set; }
        public List<string> PublicDependencies { get; set; }
        public List<string> PrivateDependencies { get; set; }
        public string hash { get; set; }

        public GModuleRules(string moduleName)
        {
            ModuleName = moduleName;
            PublicDependencies = new List<string>();
            PrivateDependencies = new List<string>();
            hash = "";
        }
    }

    class GModule 
    {
        public static string CalculateHash(string path) {
            if(!Directory.Exists(path))
            {
                Console.WriteLine("[ERROR] CalculateHash: Directory not found: " + path);
                return "";
            }

            string[] privateFiles = Directory.GetFiles(Path.Combine(path, "Private"), "*.*", SearchOption.AllDirectories);
            string[] publicFiles = Directory.GetFiles(Path.Combine(path, "Public"), "*.*", SearchOption.AllDirectories);

            string combinedString = "";
            foreach(string file in privateFiles)
            {
                Console.WriteLine("    Hashing file: " + file);
                combinedString += File.ReadAllText(file);
            }
            foreach(string file in publicFiles)
            {
                Console.WriteLine("    Hashing file: " + file);
                combinedString += File.ReadAllText(file);
            }

            using (SHA256 sha256 = SHA256.Create())
            {
                byte[] hashData = sha256.ComputeHash(System.Text.Encoding.UTF8.GetBytes(combinedString));
                string combinedHash = BitConverter.ToString(hashData).Replace("-", "").ToLowerInvariant();
                Console.WriteLine("        Combined hash: " + combinedHash);
                return combinedHash;
            }
        }

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


            var moduleRules = new GModuleRules(moduleName)
            {
                hash = CalculateHash(modDir)
            };
            //Console.WriteLine($"    Module hash: {moduleRules.hash}");

            string jsonString = JsonSerializer.Serialize(moduleRules, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(Path.Combine(modDir, moduleName + ".GModule"), jsonString);
            Console.WriteLine("------------------------------------------------");
        }

        public static void moduleInfo(string module, string dir)
        {
            string modDir = Path.Combine(dir, module);
            string modFile = Path.Combine(modDir, module + ".GModule");
            if(!File.Exists(modFile))
            {
                Console.WriteLine("[ERROR] couldn't find module " + module);
                return;
            }

            string jsonString = File.ReadAllText(modFile);
            var modFileData = JsonSerializer.Deserialize<GModuleRules>(jsonString);

            Console.WriteLine("$ Name: " + modFileData.ModuleName);
            Console.WriteLine("$ Public Dependencies: ");
            foreach(string pub in modFileData.PublicDependencies)
            {
                Console.WriteLine("    " + pub);
            } 

            Console.WriteLine("$ Private Dependencies: ");
            foreach(string priv in modFileData.PrivateDependencies) 
            {
                Console.WriteLine("    " + priv);
            }

            Console.WriteLine("$ Hash: " + modFileData.hash);
        }
    }
}