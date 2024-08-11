using Mile.Project.Helpers;
using System.Xml;

namespace Mile.Xaml.RefreshPackageVersion
{
    class Program
    {
        static void Main(string[] args)
        {
            string ProjectRootPath = GitRepository.GetRootPath();
            Console.WriteLine(ProjectRootPath);

            string NuspecFilePath = string.Format(
                @"{0}\Mile.Xaml\Mile.Xaml.nuspec",
                ProjectRootPath);

            //string VersionTag = string.Empty;
            string VersionTag = "-preview";

            string VersionString = string.Format(
                "2.3.{0}.0{1}",
                DateTime.Today.Subtract(DateTime.Parse("2021-09-12")).TotalDays,
                VersionTag);

            XmlDocument Document = new XmlDocument();
            Document.PreserveWhitespace = true;
            Document.Load(NuspecFilePath);

            XmlNode? PackageNode = Document["package"];
            if (PackageNode != null)
            {
                XmlNode? MetadataNode = PackageNode["metadata"];
                if (MetadataNode != null)
                {
                    XmlNode? VersionNode = MetadataNode["version"];
                    if (VersionNode != null)
                    {
                        VersionNode.InnerText = VersionString;
                    }
                }
            }

            Document.Save(NuspecFilePath);

            Console.WriteLine(
                "Mile.Xaml.RefreshPackageVersion task has been completed.");
            Console.ReadKey();
        }
    }
}
