using System.Xml;

namespace Mile.Xaml.ControlStyleXamlGenerator
{
    internal class Program
    {
        static void MergeResourceDictionary()
        {
            DirectoryInfo dir = new(@"D:\Projects\ProjectMile\Mile.Xaml\Mile.Xaml\SunValleyStylesReferences");
            string xmlns = "http://schemas.microsoft.com/winfx/2006/xaml/presentation";
            string xmlns_x = "http://schemas.microsoft.com/winfx/2006/xaml";
            XmlDocument genericXaml = new();
            XmlElement resourceDictionaryRoot = genericXaml.CreateElement("ResourceDictionary", xmlns);
            genericXaml.AppendChild(resourceDictionaryRoot);
            XmlElement themeDictionariesNode = genericXaml.CreateElement("ResourceDictionary.ThemeDictionaries", xmlns);
            resourceDictionaryRoot.AppendChild(themeDictionariesNode);
            XmlElement darkThemeNode = genericXaml.CreateElement("ResourceDictionary", xmlns);
            darkThemeNode.SetAttribute("Key", xmlns_x, "Default");
            themeDictionariesNode.AppendChild(darkThemeNode);
            XmlElement lightThemeNode = genericXaml.CreateElement("ResourceDictionary", xmlns);
            lightThemeNode.SetAttribute("Key", xmlns_x, "Light");
            themeDictionariesNode.AppendChild(lightThemeNode);
            XmlElement contrastThemeNode = genericXaml.CreateElement("ResourceDictionary", xmlns);
            contrastThemeNode.SetAttribute("Key", xmlns_x, "HighContrast");
            themeDictionariesNode.AppendChild(contrastThemeNode);

            foreach (FileInfo controlStyleXamlFile in dir.EnumerateFiles())
            {
                XmlDocument splittedXaml = new();
                splittedXaml.Load(controlStyleXamlFile.FullName);

                foreach (XmlAttribute ns in splittedXaml.DocumentElement.Attributes)
                {
                    if (ns.Name.StartsWith("xmlns:"))
                    {
                        resourceDictionaryRoot.SetAttributeNode(genericXaml.ImportNode(ns, true) as XmlAttribute);
                    }
                }

                foreach (XmlComment comment in splittedXaml.SelectNodes("//comment()"))
                {
                    comment.ParentNode.RemoveChild(comment);
                }

                foreach (XmlNode node in splittedXaml.DocumentElement.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "ResourceDictionary.MergedDictionaries":
                            break;
                        case "ResourceDictionary.ThemeDictionaries":
                            foreach (XmlNode themeNode in node.ChildNodes)
                            {
                                XmlNode tmpNode = null;
                                switch (themeNode.Attributes.GetNamedItem("Key", xmlns_x).Value)
                                {
                                    case "Default":
                                        tmpNode = darkThemeNode;
                                        break;
                                    case "Light":
                                        tmpNode = lightThemeNode;
                                        break;
                                    case "HighContrast":
                                        tmpNode = contrastThemeNode;
                                        break;
                                }
                                foreach (XmlNode resNode in themeNode.ChildNodes)
                                {
                                    tmpNode?.AppendChild(genericXaml.ImportNode(resNode, true));
                                }
                            }
                            break;
                        default:
                            resourceDictionaryRoot.AppendChild(genericXaml.ImportNode(node, true));
                            break;
                    }
                }
            }
            genericXaml.Save(@"D:\Projects\ProjectMile\Mile.Xaml\Mile.Xaml\SunValleyStyles.xaml");
        }

        static void Main(string[] args)
        {
            MergeResourceDictionary();

            Console.WriteLine("Hello, World!");
            Console.ReadKey();
        }
    }
}
