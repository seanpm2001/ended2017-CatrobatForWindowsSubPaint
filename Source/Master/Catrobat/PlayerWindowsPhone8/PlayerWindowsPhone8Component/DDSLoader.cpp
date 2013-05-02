#include "pch.h"
#include "DDSLoader.h"

using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace concurrency;
using namespace std;

DDSLoader::DDSLoader(vector<unsigned char> image, unsigned int width,  unsigned int height)
{
	int index = 0;
	bdata = new byte[image.size()];
	for (int index = 0; index < image.size();)
	{
		bdata[index] = image.at(index + 3);     // A
		bdata[index + 1] = image.at(index);     // R
		bdata[index + 2] = image.at(index + 1); // G
		bdata[index + 3] = image.at(index + 2); // B
		index += 4;
	}
	m_streamLength = image.size();
	m_ddsHeader.dwSize = DDSD_HEADERSIZE;
	m_ddsHeader.dwWidth = width;
	m_ddsHeader.dwHeight = height;
	m_ddsHeader.dwFlags = DDSD_CAPS || DDSD_HEIGHT || DDSD_WIDTH || DDSD_PITCH || DDSD_PIXELFORMAT;

	m_location = Package::Current->InstalledLocation; 
	m_locationPath = Platform::String::Concat(m_location->Path, "//"); 
}

void DDSLoader::WriteFile()
{
	PCWSTR SaveStateFile = L"testsave.txt";
    auto folder = ApplicationData::Current->LocalFolder;
    task<StorageFile^> getFileTask(folder->CreateFileAsync(ref new Platform::String(SaveStateFile), CreationCollisionOption::ReplaceExisting));

    auto writer = std::make_shared<Streams::DataWriter^>(nullptr);

    getFileTask.then([](StorageFile^ file)
    {
        return file->OpenAsync(FileAccessMode::ReadWrite);
    }).then([this, writer](Streams::IRandomAccessStream^ stream)
    {
        Streams::DataWriter^ state = ref new Streams::DataWriter(stream);
        *writer = state;
		
		for (int index = 0; index < m_streamLength; index++)
		{
			state->WriteByte(bdata[index]);
		}

        return state->StoreAsync();
    }).then([writer](uint32 count)
    {
        return (*writer)->FlushAsync();
    }).then([this, writer](bool flushed)
    {
        delete (*writer);
    });
}