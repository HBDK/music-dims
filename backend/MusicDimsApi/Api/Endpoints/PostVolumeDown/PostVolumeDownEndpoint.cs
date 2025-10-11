using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostVolumeDown.Gateways;

namespace MusicDimsApi.Endpoints.PostVolumeDown;

public static class PostVolumeDownEndpoint
{
    public static WebApplicationBuilder AddPostVolumeDownServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostVolumeDownServices));
        logger.LogInformation("setting up volume down");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostVolumeDownEndpoint(this WebApplication app)
    {
        app.MapPost("/players/{id}/volume-down", async ([FromServices] IHaGateway haGateway, [FromRoute] string id) =>
        {
            await haGateway.VolumeDown(id);
        }).WithName("post-volume-down");
 
        return app;
    } 
}