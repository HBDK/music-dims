using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.GetRadioChannels.Gateways;
using MusicDimsApi.Endpoints.Models;

namespace MusicDimsApi.Endpoints.GetRadioChannels;

public static class GetRadioChannelsEndpoint
{
    public static WebApplicationBuilder AddGetRadioChannelsEndpointServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddGetRadioChannelsEndpointServices));
        logger.LogInformation("setting up GetRadio");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapGetRadioChannelsEndpoint(this WebApplication app)
    {
        app.MapGet("/radio-channels", async ([FromServices] IHaGateway haGateway) =>
        {
            var values = await haGateway.GetRadio();
            var result = new NavigationResponseDto(values.ToArray(), "/");
            return result;
        }).WithName("get-radio-channels");
 
        return app;
    } 
}