// Parses an ESP32-S3 app image and maps each segment at its runtime load address.
// Run as a Ghidra postScript with the app image path as the first argument.

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;

public class LoadEsp32S3Segments extends GhidraScript {
	private int u8(byte[] b, int off) {
		return b[off] & 0xff;
	}

	private int u32le(byte[] b, int off) {
		return u8(b, off) | (u8(b, off + 1) << 8) | (u8(b, off + 2) << 16) | (u8(b, off + 3) << 24);
	}

	private boolean executable(int addr) {
		return (addr >= 0x40000000 && addr < 0x44000000) || (addr >= 0x40370000 && addr < 0x40400000);
	}

	private boolean writable(int addr) {
		return (addr >= 0x3fc00000 && addr < 0x40000000) || (addr >= 0x50000000 && addr < 0x50100000) ||
			(addr >= 0x60000000 && addr < 0x60100000);
	}

	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		if (args.length < 1) {
			printerr("Usage: LoadEsp32S3Segments <app-image.bin>");
			return;
		}

		byte[] image = Files.readAllBytes(new File(args[0]).toPath());
		if (image.length < 24 || u8(image, 0) != 0xe9) {
			throw new IOException("Not an ESP app image: missing 0xe9 magic");
		}

		int segmentCount = u8(image, 1);
		int entry = u32le(image, 4);
		int off = 24;

		for (int i = 0; i < segmentCount; i++) {
			if (off + 8 > image.length) {
				throw new IOException("Truncated segment header " + i);
			}
			int loadAddr = u32le(image, off);
			int length = u32le(image, off + 4);
			off += 8;
			if (off + length > image.length) {
				throw new IOException("Truncated segment data " + i);
			}

			String name = String.format("esp32s3_seg%d_%08x", i, loadAddr);
			Address start = toAddr(Integer.toUnsignedLong(loadAddr));
			MemoryBlock existing = currentProgram.getMemory().getBlock(start);
			if (existing == null || !existing.contains(start.add(length - 1L))) {
				ByteArrayInputStream stream = new ByteArrayInputStream(image, off, length);
				MemoryBlock block = createMemoryBlock(name, start, stream, length, false);
				block.setRead(true);
				block.setWrite(writable(loadAddr));
				block.setExecute(executable(loadAddr));
			}
			createLabel(start, name, true, SourceType.USER_DEFINED);
			off += length;
		}

		Address entryAddr = toAddr(Integer.toUnsignedLong(entry));
		createLabel(entryAddr, "esp32s3_entry", true, SourceType.USER_DEFINED);
		disassemble(entryAddr);
		createFunction(entryAddr, "esp32s3_entry");
		printf("Mapped %d ESP32-S3 segments, entry 0x%08x\n", segmentCount, entry);
	}
}
