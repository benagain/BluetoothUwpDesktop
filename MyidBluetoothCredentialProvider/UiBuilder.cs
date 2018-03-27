using CredentialProvider.Interop;
using Headspring;
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace MyidBluetoothCredentialProvider
{
    public interface UiElement
    {
        Display DisplayIn { get; }

        string Label { get; }
    }

    public class UiLabel : UiElement
    {
        public UiLabel(string label, Display displayIn)
        {
            Label = label;
            DisplayIn = displayIn;
        }

        public Display DisplayIn { get; }

        public string Label { get; }
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

        public (_CREDENTIAL_PROVIDER_FIELD_STATE, _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE) GetState(uint index)
        {
            if (index > elements.Count)
                throw new IndexOutOfRangeException($"There are not enough UI elements for index {index}");

            var element = elements[(int)index];
            return (element.DisplayIn.State, _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE.CPFIS_NONE);
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
    }
}