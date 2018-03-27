using CredentialProvider.Interop;
using Headspring;
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace MyidBluetoothCredentialProvider
{
    public interface UiElement
    {
        Display DisplayIn { get; }

        _CREDENTIAL_PROVIDER_FIELD_TYPE Type { get; }

        string Label { get; }
    }

    public class UiLabel : UiElement
    {
        public UiLabel(string label, Display displayIn)
        {
            Label = label;
            DisplayIn = displayIn;
        }

        public _CREDENTIAL_PROVIDER_FIELD_TYPE Type => _CREDENTIAL_PROVIDER_FIELD_TYPE.CPFT_SMALL_TEXT;

        public Display DisplayIn { get; }

        public string Label { get; }
    }

    class UiIcon : UiElement
    {
        public UiIcon(Display displayIn)
        {
            DisplayIn = displayIn;
        }


        public _CREDENTIAL_PROVIDER_FIELD_TYPE Type => _CREDENTIAL_PROVIDER_FIELD_TYPE.CPFT_TILE_IMAGE;

        public Display DisplayIn { get; }

        public string Label => "Icon";
    }

    public class Display : Enumeration<Display>
    {
        public static readonly Display InMenu = new Display(0, _CREDENTIAL_PROVIDER_FIELD_STATE.CPFS_DISPLAY_IN_DESELECTED_TILE);

        public static readonly Display InSelection = new Display(1, _CREDENTIAL_PROVIDER_FIELD_STATE.CPFS_DISPLAY_IN_SELECTED_TILE);

        public static readonly Display InMenuAndSelection = new Display(2, _CREDENTIAL_PROVIDER_FIELD_STATE.CPFS_DISPLAY_IN_BOTH);

        public _CREDENTIAL_PROVIDER_FIELD_STATE State{ get; }

        private Display(int v, _CREDENTIAL_PROVIDER_FIELD_STATE cPFS_DISPLAY_IN_DESELECTED_TILE, [CallerMemberName] string displayName = null)
            : base(v, displayName)
        {
            State = cPFS_DISPLAY_IN_DESELECTED_TILE;
        }
    }

    public class Ui
    {
        private readonly List<UiElement> elements;

        public Ui(List<UiElement> elements)
        {
            this.elements = elements;
        }

        public uint ElementCount => (uint)elements.Count;

        public (_CREDENTIAL_PROVIDER_FIELD_STATE, _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE) GetState(uint index)
        {
            var element = GetElementAt(index);
            return (element.DisplayIn.State, _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE.CPFIS_NONE);
        }

        internal string GetString(uint index) => GetElementAt(index).Label;

        private UiElement GetElementAt(uint index)
        {
            if (index > elements.Count)
                throw new IndexOutOfRangeException($"There are not enough UI elements for index {index}");

            return elements[(int)index];
        }

        internal void DescribeInto(uint index, IntPtr destination)
        {
            var element = GetElementAt(index);

            var descr = new _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR
            {
                dwFieldID = index,
                cpft = element.Type,
                pszLabel = element.Label,
                guidFieldType = Guid.NewGuid(),
            };

            var pcpfd = Marshal.AllocHGlobal(Marshal.SizeOf(descr)); /* _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* */
            Marshal.StructureToPtr(descr, pcpfd, false); /* pcpfd = &CredentialProviderFieldDescriptorList */
            Marshal.StructureToPtr(pcpfd, destination, false); /* *destination = pcpfd */
        }
    }


    internal class UiBuilder
    {
        private List<UiElement> elements = new List<UiElement>();

        internal UiBuilder Text(string label, Display displayIn)
        {
            elements.Add(new UiLabel(label, displayIn));
            return this;
        }

        internal Ui Build()
        {
            return new Ui(elements);
        }

        internal UiBuilder Icon(Display inMenuAndSelection)
        {
            elements.Add(new UiIcon(inMenuAndSelection));
            return this;
        }
    }
}