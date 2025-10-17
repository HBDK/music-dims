using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.GetPlayer.Gateways;
using MusicDimsApi.Endpoints.GetPlayer.Models;

namespace MusicDimsApi.Endpoints.GetPlayer;

public static class GetPlayerEndpoint
{
    public static WebApplicationBuilder AddGetPlayerEndpoint(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddGetPlayerEndpoint));
        logger.LogInformation("setting up get player state");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapGetPlayerEndpoint(this WebApplication app)
    {
        app.MapGet("/players/{id}", async ([FromServices] IHaGateway haGateway, [FromRoute] string id) =>
        {
            var raw = await haGateway.GetState(id);
            return new GetPlayerStateResponseDto
            {
                State = raw.State,
                Volume = (int)((raw.AttributesInfo?.VolumeLevel ?? 0)*100),
                Muted = raw.AttributesInfo?.IsVolumeMuted ?? false,
                Title = raw.AttributesInfo?.MediaTitle ?? string.Empty,
                Artist = raw.AttributesInfo?.MediaArtist ?? string.Empty,
                Album = raw.AttributesInfo?.MediaAlbumName ?? string.Empty,
                PictureUrl = raw.AttributesInfo?.EntityPicture?.Replace($"/api/media_player_proxy/media_player.{id}?", "") ?? string.Empty
            };
        }).WithName("get-player-state");
 
        return app;
    } 
}