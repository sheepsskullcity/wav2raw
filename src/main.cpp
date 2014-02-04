#include "WaveFile.h"

#include <iostream>
#include <string>
#include <fstream>

void print_usage()
{
	std::cout << "Usage: wav2raw file1.wav [file2.wav] [file3.wav] ..." << std::endl;
}

void process_file(const std::string &filename)
{
	std::cout << "Processing \"" << filename << "\"..." << std::endl;
	WaveFile wavFile(filename);
	if (wavFile.IsLoaded())
	{
		std::string audioFormat = "Unknown";
		switch (wavFile.GetAudioFormat())
		{
		case WaveFile::PCM: audioFormat = "PCM"; break;
		}

		std::cout <<
			"Meta data:\n"
			" - Audio format : " << audioFormat << "\n"
			" - Channels     : " << wavFile.GetNumChannels() << "\n"
			" - Sample rate  : " << wavFile.GetSampleRate() << "\n"
			" - Sample size  : " << wavFile.GetBitsPerSample() << " bits" << std::endl;

		std::string filenameSansExt = filename.substr(0, filename.find_last_of('.'));
		std::string filenameRaw = filenameSansExt + ".raw";
		std::fstream rawFile(filenameRaw, std::ios::out | std::ios::trunc | std::ios::binary);

		if (!rawFile.is_open())
		{
			std::cerr << "Error: Failed to open output file \"" << filenameRaw << "\"." << std::endl;
			return;
		}

		const char *rawData = wavFile.GetData();
		std::size_t rawSize = wavFile.GetDataSize();
		rawFile.write(rawData, rawSize);
		std::cout << "Wrote \"" << filenameRaw << "\"." << std::endl;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		print_usage();
		return 0;
	}

	for (int i = 1; i < argc; ++i)
	{
		std::string filename = argv[i];
		process_file(filename);
	}

	return 0;
}