﻿using System;
using System.Collections.Generic;
using System.Linq;

namespace BluetoothTransportProtocolTests
{
    public class PacketEventArgs : EventArgs
    {
        public PacketEventArgs(byte[] bytes)
        {
            Bytes = bytes;
        }

        public byte[] Bytes { get; }
    }

    public class PacketBuilder
    {
        private List<byte[]> blocks = new List<byte[]>();

        public event EventHandler<PacketEventArgs> OnCompletedPacket;

        internal void AddBlock(byte[] block)
        {
            if (block.SequenceEqual(System.Text.Encoding.UTF8.GetBytes("EOM")))
            {
                OnCompletedPacket.Invoke(this, new PacketEventArgs(GetCompletedPacket()));
            }
            else
            {
                blocks.Add(block);
            }
        }

        internal byte[] GetCompletedPacket()
        {
            int count = blocks.Sum(x => x.Length);
            byte[] fullPacket = new byte[count];
            int destinationIndex = 0;

            foreach(var block in blocks)
            {
                Array.Copy(block, 0, fullPacket, destinationIndex, block.Length);
                destinationIndex += block.Length;
            }

            return fullPacket;
        }
    }
}