using Serilog;
using Serilog.Configuration;
using Serilog.Core;
using Serilog.Events;
using System.Diagnostics;
using System.Linq;

namespace BluetoothUwpDesktop
{
    class CallerEnricher : ILogEventEnricher
    {
        public void Enrich(LogEvent logEvent, ILogEventPropertyFactory propertyFactory)
        {
            var skip = 3;
            while (true)
            {
                var method = new StackFrame(skip).GetMethod();
                if (method == null)
                    return;

                if (method.DeclaringType.Assembly != typeof(Log).Assembly)
                {
                    logEvent.AddPropertyIfAbsent(new LogEventProperty("Caller", new ScalarValue(method.Name)));
                }

                skip++;
            }
        }
    }

    static class LoggerCallerEnrichmentConfiguration
    {
        public static LoggerConfiguration WithCaller(this LoggerEnrichmentConfiguration enrichmentConfiguration)
        {
            return enrichmentConfiguration.With<CallerEnricher>();
        }
    }
}
