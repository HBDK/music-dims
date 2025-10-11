using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.PostVolumeUp.Gateways;

namespace MusicDimsApi.Endpoints.PostVolumeUp;

public static class PostVolumeUpEndpoint
{
    public static WebApplicationBuilder AddPostVolumeUpServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddPostVolumeUpServices));
        logger.LogInformation("setting up volume up");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapPostVolumeUpEndpoint(this WebApplication app)
    {
        app.MapPost("/players/{id}/volume-up", async ([FromServices] IHaGateway haGateway, [FromRoute] string id) =>
        {
            await haGateway.VolumeUp(id);
        }).WithName("post-volume-up");
 
        return app;
    } 
}