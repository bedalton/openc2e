/*
 *  PrayFileReader.cpp
 *  openc2e
 *
 *  Created by Alyssa Milburn on Mon Jan 16 2006.
 *  Copyright (c) 2006 Alyssa Milburn. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#include "PrayFileReader.h"

#include "common/Exception.h"
#include "common/encoding.h"
#include "common/endianlove.h"
#include "common/spanstream.h"

#include <cstring>
#include <zlib.h>

PrayFileReader::PrayFileReader(std::istream& stream_)
	: stream(stream_) {
	char majic[4];
	stream.read(majic, 4);
	if (strncmp(majic, "PRAY", 4) != 0)
		throw Exception("bad magic of PRAY file");

	while (true) {
		stream.peek();
		if (stream.eof()) {
			break;
		}
		if (!stream) {
			throw Exception("Stream failure while reading PRAY file");
		}

		block_offsets.push_back(stream.tellg());

		stream.seekg(132, std::ios::cur); // skip type and name
		uint32_t compressedsize = read32le(stream);
		stream.seekg(8 + compressedsize, std::ios::cur);
	}
}

PrayFileReader::~PrayFileReader() {
}

size_t PrayFileReader::getNumBlocks() {
	return block_offsets.size();
}

std::string PrayFileReader::getBlockType(size_t i) {
	stream.seekg(block_offsets[i]);
	char type[5];
	type[4] = 0;
	stream.read(type, 4);
	return type;
}

std::string PrayFileReader::getBlockName(size_t i) {
	stream.seekg(block_offsets[i] + 4);
	char name[129];
	name[128] = 0;
	stream.read(name, 128);
	return ensure_utf8(name);
}

bool PrayFileReader::getBlockIsCompressed(size_t i) {
	stream.seekg(block_offsets[i] + 140);
	uint32_t flags = read32le(stream);
	return ((flags & 1) == 1);
}

std::vector<unsigned char> PrayFileReader::getBlockRawData(size_t i) {
	std::string name = getBlockName(i);

	stream.seekg(block_offsets[i] + 132);
	uint32_t compressedsize = read32le(stream);
	uint32_t size = read32le(stream);
	uint32_t flags = read32le(stream);
	bool compressed = ((flags & 1) == 1);

	if (!compressed && size != compressedsize)
		throw Exception("Size doesn't match compressed size for uncompressed block.");

	std::vector<unsigned char> buffer(size);

	if (compressed) {
		// TODO: check pray_uncompress_sanity_check
		std::vector<unsigned char> src(compressedsize);
		stream.read((char*)src.data(), compressedsize);
		if (!stream.good()) {
			throw Exception("Failed to read all of compressed block.");
		}
		uLongf usize = size;
		int r = uncompress((Bytef*)buffer.data(), (uLongf*)&usize, (Bytef*)src.data(), compressedsize);
		if (r != Z_OK) {
			std::string o = "Unknown error";
			switch (r) {
				case Z_MEM_ERROR: o = "Out of memory"; break;
				case Z_BUF_ERROR: o = "Out of buffer space"; break;
				case Z_DATA_ERROR: o = "Corrupt data"; break;
			}
			o = o + " while decompressing PRAY block \"" + name + "\"";
			throw Exception(o);
		}
		if (usize != size) {
			throw Exception("Decompressed data is not the correct size.");
		}
	} else {
		stream.read((char*)buffer.data(), size);
		if (!stream.good()) {
			throw Exception("Failed to read all of uncompressed block.");
		}
	}

	return buffer;
}

static std::string tagStringRead(std::istream& in) {
	unsigned int len = read32le(in);

	std::string data(len, '0');
	in.read(&data[0], len);
	return data;
}

std::pair<std::map<std::string, uint32_t>, std::map<std::string, std::string>> PrayFileReader::getBlockTags(size_t i) {
	std::string name = getBlockName(i);

	std::map<std::string, uint32_t> integerValues;
	std::map<std::string, std::string> stringValues;

	auto buffer = getBlockRawData(i);
	spanstream s(buffer);

	unsigned int nointvalues = read32le(s);
	for (unsigned int i = 0; i < nointvalues; i++) {
		std::string n = ensure_utf8(tagStringRead(s));
		unsigned int v = read32le(s);
		if (integerValues.find(n) == integerValues.end()) {
			integerValues[n] = v;
		} else if (integerValues[n] != v) {
			throw Exception(std::string("Duplicate tag \"") + n + "\"");
		}
	}

	unsigned int nostrvalues = read32le(s);
	for (unsigned int i = 0; i < nostrvalues; i++) {
		std::string n = ensure_utf8(tagStringRead(s));
		std::string v = ensure_utf8(tagStringRead(s));
		if (stringValues.find(n) == stringValues.end()) {
			stringValues[n] = v;
		} else if (stringValues[n] != v) {
			throw Exception(std::string("Duplicate tag \"") + n + "\"");
		}
	}

	if (!s) {
		throw Exception("Stream failure reading tags from PRAY block \"" + name + "\"");
	}
	s.peek();
	if (!s.eof()) {
		throw Exception("Didn't read whole block while reading tags from PRAY block \"" + name + "\"");
	}

	return {integerValues, stringValues};
}
