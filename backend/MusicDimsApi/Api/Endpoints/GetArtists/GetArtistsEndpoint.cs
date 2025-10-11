using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.GetArtists.Gateways;

namespace MusicDimsApi.Endpoints.GetArtists;

public static class GetArtistsEndpoint
{
    public static WebApplicationBuilder AddGetArtistsEndpointServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddGetArtistsEndpointServices));
        logger.LogInformation("setting up GetArtists");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapGetArtistsEndpoint(this WebApplication app)
    {
        app.MapGet("/artists", async ([FromServices] IHaGateway haGateway) =>
        {
            var values = await haGateway.GetLibrary();
            return values;
        }).WithName("get-artists");
 
        return app;
    } 
}