using Microsoft.AspNetCore.Mvc;
using MusicDimsApi.Endpoints.GetArtistAlbums.Gateways;
using MusicDimsApi.Endpoints.Models;

namespace MusicDimsApi.Endpoints.GetArtistAlbums;

public static class GetArtistAlbumsEndpoint
{
    public static WebApplicationBuilder AddGetArtistAlbumsEndpointServices(this WebApplicationBuilder builder)
    {
        var loggerFactory = LoggerFactory.Create(x => x.AddConsole());
        var logger = loggerFactory.CreateLogger(nameof(AddGetArtistAlbumsEndpointServices));
        logger.LogInformation("setting up GetArtistAlbums");

        builder.Services.AddScoped<IHaGateway, HaGateway>();
        return builder;
    }

    public static WebApplication MapGetArtistAlbumsEndpoint(this WebApplication app)
    {
        app.MapGet("/artists/{id}/albums", async ([FromServices] IHaGateway haGateway, [FromRoute]string id) =>
        {
            var values = await haGateway.GetAlbums(id);
            var result = new NavigationResponseDto(values.ToArray(), "/artists");
            return result;
        }).WithName("get-artist-albums");
 
        return app;
    } 
}